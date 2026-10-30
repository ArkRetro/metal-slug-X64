/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087008.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87008(void)

{
  char cVar1;
  ushort uVar2;
  undefined2 uVar3;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  uVar2 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x82) = (uVar2 & 0x1c) + 8;
  candidate_878d2();
  candidate_1af9a();
  *(undefined4 *)((int)unaff_A6 + 0x6e) = extraout_A0;
  uVar3 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x20) = uVar3;
  candidate_4256();
  *unaff_A6 = 0x8704c;
  uVar4 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_86c08;
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
  bVar5 = false;
  candidate_4c24();
  if ((bVar5) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_871aa;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar5 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
