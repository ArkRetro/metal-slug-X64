/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09c5ba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9c5ba(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
  unaff_A6[0x14] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + -8;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 0x12;
  unaff_A6[0xd] = 0x7ae14;
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0xffff62b7;
  unaff_A6[0xc] = 0x3d70a;
  candidate_4256();
  *unaff_A6 = 0x9c60c;
  uVar1 = false;
  candidate_9cca0();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9c636;
    uVar1 = 0;
  }
  candidate_427a();
  candidate_19504();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
