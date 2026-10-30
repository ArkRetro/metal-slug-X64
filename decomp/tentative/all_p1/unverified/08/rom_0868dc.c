/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0868dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_868dc(void)

{
  char cVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar3;

  if ((*(byte *)((int)unaff_A6 + 0x8b) & 2) != 0) {
    candidate_87b3a();
    if ((bool)in_CF) {
      unaff_A6[0xc] = 0;
      in_CF = 0;
    }
  }
  func_0x000143e4();
  candidate_1ad8c();
  candidate_427a();
  candidate_87b30();
  if ((bool)in_CF) {
    candidate_87bb8();
    *(char *)((int)unaff_A6 + 0x89) = *(char *)((int)unaff_A6 + 0x89) + '\x01';
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_86f04;
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
  bVar3 = false;
  candidate_4c24();
  if ((bVar3) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_871aa;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
