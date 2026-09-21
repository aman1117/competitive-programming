"""Local, append-only practice history; no LeetCode API, credentials, or scraping."""

import argparse
from contextlib import contextmanager
from datetime import date, timedelta
import json
from pathlib import Path
import sqlite3

INTERVALS = (2, 7, 21)
OUTCOMES = ("independent", "hinted", "editorial", "failed")
MISTAKES = ("none", "recognition", "proof", "implementation", "complexity", "edge-case", "overflow", "time")


def day(value):
    try:
        return date.fromisoformat(value)
    except ValueError:
        raise argparse.ArgumentTypeError("Use an ISO date: YYYY-MM-DD.") from None


def positive_minutes(value):
    number = int(value)
    if number < 0:
        raise argparse.ArgumentTypeError("Minutes must not be negative.")
    return number


@contextmanager
def database(filename):
    path = Path(filename).resolve()
    path.parent.mkdir(parents=True, exist_ok=True)
    connection = sqlite3.connect(path, timeout=10)
    connection.row_factory = sqlite3.Row
    try:
        connection.execute("PRAGMA foreign_keys = ON")
        version = connection.execute("PRAGMA user_version").fetchone()[0]
        if version not in (0, 1):
            raise ValueError(f"Unsupported revision database version: {version}")
        if version == 0 and connection.execute(
            "SELECT 1 FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%' LIMIT 1"
        ).fetchone():
            raise ValueError("Refusing to initialize an unrelated SQLite database; choose a new --db path.")
        connection.executescript("""
            CREATE TABLE IF NOT EXISTS problems (
                platform TEXT NOT NULL, problem TEXT NOT NULL, title TEXT NOT NULL,
                topic TEXT NOT NULL, last_date TEXT NOT NULL, stage INTEGER NOT NULL,
                due TEXT, PRIMARY KEY(platform,problem)
            );
            CREATE TABLE IF NOT EXISTS attempts (
                id INTEGER PRIMARY KEY, platform TEXT NOT NULL, problem TEXT NOT NULL,
                attempted TEXT NOT NULL, outcome TEXT NOT NULL, minutes INTEGER NOT NULL,
                mistake TEXT NOT NULL, note TEXT NOT NULL,
                FOREIGN KEY(platform,problem) REFERENCES problems(platform,problem)
            );
            PRAGMA user_version = 1;
        """)
        yield connection
        connection.commit()
    finally:
        connection.close()


def record_attempt(filename, *, platform, problem, title, topic, attempted, outcome, minutes, mistake, note):
    if not problem.strip() or any(ch.isspace() for ch in problem):
        raise ValueError("Problem ID must be nonempty and contain no whitespace (examples: 239, 1899C).")
    with database(filename) as connection:
        # Serialize the read/modify/write transaction when two terminals log together.
        connection.execute("BEGIN IMMEDIATE")
        old = connection.execute("SELECT * FROM problems WHERE platform=? AND problem=?",
                                 (platform, problem)).fetchone()
        if old and attempted.isoformat() < old["last_date"]:
            raise ValueError("Backdated attempts are not supported; use chronological dates.")
        stage = old["stage"] if old else 0
        if outcome != "independent":
            stage = 0
        elif old and old["due"] and attempted.isoformat() >= old["due"]:
            stage = min(stage + 1, len(INTERVALS))
        # Early independent repeats do not consume spaced-review stages.
        if old and outcome == "independent" and old["due"] and attempted.isoformat() < old["due"]:
            due = old["due"]
        else:
            due = (attempted + timedelta(days=INTERVALS[stage])).isoformat() if stage < len(INTERVALS) else None
        title = title or (old["title"] if old else problem)
        topic = topic or (old["topic"] if old else "unclassified")
        connection.execute("""
            INSERT INTO problems VALUES (?,?,?,?,?,?,?)
            ON CONFLICT(platform,problem) DO UPDATE SET title=excluded.title,
                topic=excluded.topic, last_date=excluded.last_date, stage=excluded.stage, due=excluded.due
        """, (platform, problem, title, topic, attempted.isoformat(), stage, due))
        connection.execute("""
            INSERT INTO attempts(platform,problem,attempted,outcome,minutes,mistake,note)
            VALUES (?,?,?,?,?,?,?)
        """, (platform, problem, attempted.isoformat(), outcome, minutes, mistake, note))
    return due, stage


def log_command(args):
    due, stage = record_attempt(
        args.db, platform=args.platform, problem=args.problem, title=args.title, topic=args.topic,
        attempted=args.date, outcome=args.outcome, minutes=args.minutes, mistake=args.mistake, note=args.note,
    )
    print(f"Recorded {args.platform.upper()} {args.problem}. "
          + (f"Next review: {due} (stage {stage + 1}/3)." if due else "Spaced cycle complete; retain in mixed practice."))
    return 0


def due_command(args):
    if not Path(args.db).exists():
        print("No practice history yet. Use cptool log after solving on the platform.")
        return 0
    with database(args.db) as connection:
        rows = connection.execute("""
            SELECT platform,problem,title,topic,due FROM problems
            WHERE due IS NOT NULL AND due <= ? ORDER BY due,platform,problem
        """, (args.date.isoformat(),)).fetchall()
    if args.json:
        print(json.dumps([dict(row) for row in rows], indent=2))
    elif not rows:
        print(f"No reviews due by {args.date}.")
    else:
        for row in rows:
            print(f"{row['due']}  {row['platform'].upper()} {row['problem']}  {row['title']}  [{row['topic']}]")
    return 0


def history_command(args):
    if not Path(args.db).exists():
        print("No practice history yet.")
        return 0
    with database(args.db) as connection:
        condition, parameters = ("WHERE platform=? AND problem=?", (args.platform, args.problem)) if args.problem else ("", ())
        rows = connection.execute(f"SELECT * FROM attempts {condition} ORDER BY attempted DESC,id DESC LIMIT 100",
                                  parameters).fetchall()
    print(json.dumps([dict(row) for row in rows], indent=2))
    return 0


def stats_command(args):
    if not Path(args.db).exists():
        print("No practice history yet.")
        return 0
    with database(args.db) as connection:
        total = connection.execute("SELECT COUNT(*) FROM problems").fetchone()[0]
        attempts = connection.execute("SELECT outcome,COUNT(*) AS count FROM attempts GROUP BY outcome").fetchall()
        errors = connection.execute("""
            SELECT mistake,COUNT(*) AS count FROM attempts WHERE mistake!='none'
            GROUP BY mistake ORDER BY count DESC,mistake
        """).fetchall()
    print(f"{total} distinct problems. Outcomes are self-reported, not platform-synced.")
    for row in attempts:
        print(f"  {row['outcome']}: {row['count']}")
    print("Mistake categories:")
    for row in errors:
        print(f"  {row['mistake']}: {row['count']}")
    return 0


def register_commands(commands, root):
    default = str(root / ".practice" / "revision.sqlite3")
    log = commands.add_parser("log", help="Record an on-platform attempt and schedule spaced revision.")
    log.add_argument("problem", help="Problem ID (LC by default), e.g. 239 or 1899C.")
    log.add_argument("--platform", choices=["lc", "cf"], default="lc")
    log.add_argument("--title")
    log.add_argument("--topic")
    log.add_argument("--outcome", choices=OUTCOMES, required=True)
    log.add_argument("--minutes", type=positive_minutes, default=0)
    log.add_argument("--mistake", choices=MISTAKES, default="none")
    log.add_argument("--note", default="", help="The missed invariant or corrective lesson; do not enter secrets.")
    log.add_argument("--date", type=day, default=date.today())
    log.add_argument("--db", default=default)
    log.set_defaults(handler=log_command)
    due = commands.add_parser("due", help="Show reviews due today (or by --date).")
    due.add_argument("--date", type=day, default=date.today())
    due.add_argument("--json", action="store_true")
    due.add_argument("--db", default=default)
    due.set_defaults(handler=due_command)
    history = commands.add_parser("history", help="Show the last 100 logged attempts as JSON.")
    history.add_argument("problem", nargs="?")
    history.add_argument("--platform", choices=["lc", "cf"], default="lc")
    history.add_argument("--db", default=default)
    history.set_defaults(handler=history_command)
    stats = commands.add_parser("stats", help="Summarize self-reported outcomes and mistakes.")
    stats.add_argument("--db", default=default)
    stats.set_defaults(handler=stats_command)
