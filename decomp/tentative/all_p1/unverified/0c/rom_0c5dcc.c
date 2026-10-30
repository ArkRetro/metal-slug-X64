/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c5dcc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c5dcc(void)

{
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar1;
  undefined8 uVar2;

  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x97) = 5;
  *(short *)(unaff_A6 + 0xc) = *(short *)(unaff_A6 + 0xc) >> 1;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff0;
  candidate_4256();
  candidate_13f1e();
  candidate_12192();
  *unaff_A6 = 0xc5e0c;
  unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
  candidate_1ac38();
  candidate_427a();
  uVar1 = false;
  if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
    uVar2 = candidate_3c55c();
    if ((bool)uVar1) {
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
      candidate_411c(uVar2,extraout_A0);
      uVar1 = 0;
      candidate_1ab98();
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_c66e8;
  }
  return;
}
