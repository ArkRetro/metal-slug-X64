/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07626e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7626e(void)

{
  uint uVar1;
  undefined2 uVar2;
  short sVar3;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  candidate_1ad52();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  unaff_A6[0xd] = 0;
  uVar1 = candidate_29c6();
  unaff_A6[0xc] = (uVar1 & 0xffff) * 2 + -0x60000;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
  if (*(char *)(unaff_A6 + 0x1c) == '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  unaff_A6[0x14] = 0xe3acc;
  candidate_4256();
  *unaff_A6 = 0x762f4;
  candidate_143dc();
  candidate_7692e();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    candidate_763a6();
  }
  bVar4 = false;
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    candidate_76844();
    if ((bVar4) &&
       (*(undefined1 *)(unaff_A6 + 0x18) = 0xff, 1 < *(short *)(*(int *)(unaff_A6[3] + 0xc) + 0x7c))
       ) {
      candidate_40bc();
      *(undefined2 *)(extraout_A0 + 0x1e) = 0x1001;
      *(undefined2 *)(extraout_A0 + 0x2c) = 0x120;
    }
  }
  candidate_768f4();
  candidate_427a();
  sVar3 = candidate_49e8();
  uVar5 = 0;
  if (sVar3 != 0) {
    candidate_763a6();
  }
  candidate_4c24();
  if ((bool)uVar5) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_763a6();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar5 = 0;
  }
  candidate_1945c();
  if ((bool)uVar5) {
    *unaff_A6 = 0x765d4;
  }
  return;
}
