/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087598.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87598(void)

{
  short sVar1;
  undefined2 uVar2;
  byte bVar3;
  byte bVar4;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar2;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
  *unaff_A6 = 0x875be;
  func_0x000143e4();
  candidate_427a();
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0x20) < 1) {
    *unaff_A6 = candidate_87640;
  }
  *(char *)(unaff_A6 + 0x22) = *(char *)(unaff_A6 + 0x22) + -1;
  uVar6 = 0;
  if (*(char *)(unaff_A6 + 0x22) < '\x01') {
    uVar6 = candidate_260c8();
    *(undefined1 *)(unaff_A6 + 0x22) = uVar6;
    candidate_1af9a();
    bVar3 = candidate_2eb4();
    bVar4 = *(byte *)((int)unaff_A6 + 0x8a);
    uVar6 = bVar3 < bVar4;
    if (bVar3 != bVar4) {
      if ((char)bVar3 < (char)bVar4) {
        bVar4 = 0xfc;
      }
      else {
        bVar4 = 4;
      }
      bVar3 = *(byte *)((int)unaff_A6 + 0x8a);
      uVar6 = CARRY1(bVar4,bVar3);
      *(byte *)((int)unaff_A6 + 0x8a) = bVar4 + bVar3;
      candidate_878a2();
    }
  }
  candidate_1af9a();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_87640;
  }
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_87674;
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_87674;
  }
  bVar5 = false;
  candidate_19504();
  if (bVar5) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
