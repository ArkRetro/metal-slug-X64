/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03eb18.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3eb18(void)

{
  undefined1 uVar1;
  undefined4 *unaff_A6;
  undefined8 uVar2;

  uVar1 = candidate_2eb4();
  *(undefined1 *)(unaff_A6 + 0x22) = uVar1;
  uVar2 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar2;
  if ((int)unaff_A6[0xc] < 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(undefined2 *)(unaff_A6 + 0x1d) = 4;
  *unaff_A6 = 0x3eb7e;
  candidate_3fa46();
  candidate_1abc8();
  candidate_1ad8c();
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) < 1) {
    *unaff_A6 = 0x3ea2c;
  }
  candidate_427a();
  candidate_3fada();
  return;
}
