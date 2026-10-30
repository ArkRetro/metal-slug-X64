/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x025362.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_25362(void)

{
  int iVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined8 uVar4;

  candidate_9f88();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_4256();
  if (*(char *)(unaff_A6[3] + 0x8a) == '\0') {
    candidate_260c8();
  }
  else if (*(char *)(unaff_A6[3] + 0x8a) == '\x01') {
    candidate_260c8();
  }
  else {
    candidate_260c8();
  }
  uVar4 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar4;
  *(undefined2 *)(unaff_A6 + 0x26) = 0;
  unaff_A6[0x20] = 0;
  *unaff_A6 = 0x2542a;
  iVar1 = unaff_A6[0xd];
  if (iVar1 < 0) {
    iVar1 = -iVar1;
  }
  unaff_A6[0x20] = iVar1 + unaff_A6[0x20];
  if (0x13ffff < (int)unaff_A6[0x20]) {
    unaff_A6[0x20] = unaff_A6[0x20] + -0x140000;
    candidate_40bc();
    candidate_1acf2();
  }
  *(undefined2 *)(unaff_A6 + 0x24) = 0xfed;
  func_0x000143e4();
  candidate_427a();
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + 1;
  if ((*(ushort *)(unaff_A6 + 0x26) & 1) == 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  }
  else {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  }
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    candidate_120f8();
    *unaff_A6 = candidate_1c71e;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = thunk_FUN_000256d4;
  }
  bVar3 = false;
  candidate_24acc();
  if (bVar3) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    *unaff_A6 = candidate_1c71e;
  }
  candidate_248d8();
  return;
}
