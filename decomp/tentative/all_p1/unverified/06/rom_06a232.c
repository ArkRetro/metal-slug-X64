/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06a232.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6a232(void)

{
  char cVar1;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_1af9a();
  *(undefined4 *)((int)unaff_A6 + 0x72) = extraout_A0;
  *unaff_A6 = 0x6a242;
  uVar2 = 0;
  candidate_1abc8();
  candidate_6b3fc();
  candidate_6b7fc();
  if ((bool)uVar2) {
    candidate_6b82e();
    if ((bool)uVar2) {
      candidate_6b8e0();
      candidate_1b1dc();
      *(undefined1 *)((int)unaff_A6 + 0x95) = 0;
                    /* WARNING: Could not recover jumptable at 0x0006a2d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x95) << 2) + 0x6a2d4))();
      return;
    }
  }
  if (*(short *)((int)unaff_A6 + 0x82) != 0) {
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  }
  if (*(char *)(unaff_A6 + 0x24) != '\0') {
    *(char *)(unaff_A6 + 0x24) = *(char *)(unaff_A6 + 0x24) + -1;
  }
  uVar2 = false;
  if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x91);
    uVar2 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x91) = cVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_6af44;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = 0x6b27e;
  }
  return;
}
