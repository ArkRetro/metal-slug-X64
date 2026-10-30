/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6500.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6500(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x76) = uVar2;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x72) = uVar1;
  *(undefined4 *)((int)unaff_A6 + 0x92) = 0xe9234;
  candidate_9ea8();
  candidate_4520();
  candidate_4256();
  *unaff_A6 = 0xd6568;
  uVar2 = 0;
  candidate_d7126();
  candidate_44f6();
  candidate_1abc8();
  candidate_427a();
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x40) == 0) {
    *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
    uVar2 = 0;
    if (*(short *)((int)unaff_A6 + 0x72) < 1) {
      candidate_d726e();
      uVar1 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x72) = uVar1;
      *(char *)((int)unaff_A6 + 0x76) = *(char *)((int)unaff_A6 + 0x76) + -1;
      uVar2 = 0;
      if (*(char *)((int)unaff_A6 + 0x76) < '\x01') {
        uVar2 = candidate_260c8();
        *(undefined1 *)((int)unaff_A6 + 0x76) = uVar2;
        uVar1 = candidate_260c8();
        *(undefined2 *)((int)unaff_A6 + 0x72) = uVar1;
        uVar2 = 0;
      }
    }
  }
  candidate_409a();
  if (!(bool)uVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
