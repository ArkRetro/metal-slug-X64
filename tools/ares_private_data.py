"""A private copy of the ares data directory for one test run.

The optional life bar (Z in the test menu) and the grenade need N64 Z, which
ares reads from VirtualPad1 R-Trigger (docs/MISION5.md); the shared settings
(/workspace/emulator/data146/ares/settings.bml) may leave it empty, and the
shared file must keep its own entries (docs/AGENTES.md). The run gets a copy
under its output directory with only that entry set to the key z
(0x1/0/60) when it is empty; the shared file is never written."""
import pathlib, shutil


def private_data(src, out):
    priv = pathlib.Path(out) / 'aresdata'
    shutil.rmtree(priv, ignore_errors=True)
    shutil.copytree(src, priv)
    sp = priv / 'ares' / 'settings.bml'
    st = sp.read_text()
    i = st.index('VirtualPad1')
    j = st.index('R-Trigger:', i)
    if st.startswith('R-Trigger: ;;', j):
        st = st[:j] + 'R-Trigger: 0x1/0/60;;' + st[j + len('R-Trigger: ;;'):]
    sp.write_text(st)
    return str(priv)
