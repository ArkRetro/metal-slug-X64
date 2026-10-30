/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03603c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3603c(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x7a);
  unaff_A6[0xd] = *(undefined4 *)((int)unaff_A6 + 0x7e);
  uVar2 = 0;
  candidate_1ac38();
  candidate_364d2();
  candidate_35af0();
  if ((bool)uVar2) {
    candidate_120f8();
    if (*(char *)((int)unaff_A6 + 0x9f) == '\a') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = candidate_360a8;
    return;
  }
  func_0x0003623e();
  sVar1 = *(short *)((int)unaff_A6 + 0x8e) + -1;
  *(short *)((int)unaff_A6 + 0x8e) = sVar1;
  if (-1 < sVar1) {
    return;
  }
  candidate_35b1a();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  candidate_36486();
  *unaff_A6 = candidate_35fd0;
  return;
}
