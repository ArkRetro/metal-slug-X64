/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x086f98.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_86f98(void)

{
  char cVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  *(char *)(unaff_A6 + 0x22) = *(char *)(unaff_A6 + 0x22) + -1;
  candidate_120f8();
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
  candidate_4256();
  *unaff_A6 = 0x86fcc;
  uVar3 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar3) {
    if (*(char *)(unaff_A6 + 0x22) < '\x01') {
      *unaff_A6 = candidate_87008;
    }
    else {
      *(char *)((int)unaff_A6 + 0x89) = *(char *)((int)unaff_A6 + 0x89) + '\x01';
      if (*(short *)(unaff_A6 + 0x20) < 1) {
        *unaff_A6 = candidate_86f98;
      }
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = 0x86eb4;
  }
  cVar1 = *(char *)((int)unaff_A6 + 0x87);
  *(char *)((int)unaff_A6 + 0x87) = cVar1 + -1;
  if (cVar1 == '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0x20) < 0) {
    *(undefined2 *)(unaff_A6 + 0x20) = 0;
  }
  bVar4 = false;
  candidate_4c24();
  if ((bVar4) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_871aa;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
