# Regenerates assets/js/md-content.js from assets/txt/**/*.md
from pathlib import Path
import json

root = Path(__file__).resolve().parents[2]
txt_root = root / "assets" / "txt"
out = root / "assets" / "js" / "md-content.js"

content = {}
for path in sorted(txt_root.rglob("*.md")):
    text = path.read_text(encoding="utf-8")
    rel = path.relative_to(root).as_posix()
    content[rel] = text
    content[f"../{rel}"] = text
    content[f"/{rel}"] = text
    content[path.name] = text

out.write_text(
    "window.MD_CONTENT = "
    + json.dumps(content, ensure_ascii=False, indent=2)
    + ";\n",
    encoding="utf-8",
)
print(f"wrote {out.relative_to(root)} ({len(content)} keys)")
