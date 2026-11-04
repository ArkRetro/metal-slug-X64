#!/usr/bin/env python3
"""Package tracked source/evidence, excluding demos, ROMs, assets and build files."""
import argparse,hashlib,json,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
assert not subprocess.check_output(['git','status','--porcelain','--untracked-files=no'],cwd=ROOT).strip(),'Commit tracked changes before packaging'
commit=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip()
tracked=subprocess.check_output(['git','ls-files','-z'],cwd=ROOT).decode().split('\0')
prefixes={'src','tools','decomp','data','docs','.github'}
root_names={'Makefile','README.md','SOURCE_STATUS.md','LICENSE','LICENSE.md','.gitignore'}
extensions={'.asm','.cmd','.inc','.c','.h','.cpp','.hpp','.S','.s','.py','.lua','.java','.sh','.mk','.md','.txt','.json','.csv','.tsv','.toml','.yml','.yaml'}
selected=[name for name in tracked if name and (name in root_names or (name.split('/')[0] in prefixes and Path(name).suffix in extensions))]
assert 'SOURCE_STATUS.md' in selected and 'src/neo_player_query.c' in selected
manifest=dict(commit=commit,decompilation_complete=False,package='Current partial source recovery and port sources',excludes=['Game ROMs/BIOS','Demo and regression ROMs','Original graphics/audio assets','Build products and SDK'],files={})
folder='metal-slug-X64-fuentes-parciales/'
a.output.parent.mkdir(parents=True,exist_ok=True)
with zipfile.ZipFile(a.output,'x',compression=zipfile.ZIP_DEFLATED,compresslevel=9) as archive:
    for name in sorted(selected):
        path=ROOT/name;assert path.is_file() and not path.is_symlink(),name
        data=path.read_bytes();manifest['files'][name]=hashlib.sha256(data).hexdigest();archive.writestr(folder+name,data)
    archive.writestr(folder+'PACKAGE_MANIFEST.json',json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(a.output) as archive:
    assert archive.testzip() is None
    for name,expected in manifest['files'].items():assert hashlib.sha256(archive.read(folder+name)).hexdigest()==expected
sha=hashlib.sha256(a.output.read_bytes()).hexdigest()
a.output.with_suffix('.zip.sha256').write_text(sha+'  '+a.output.name+'\n')
print(json.dumps(dict(path=str(a.output),commit=commit,files=len(selected),bytes=a.output.stat().st_size,sha256=sha,decompilation_complete=False),indent=2))
