/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x022f70.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_22f70(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar1;

  if ((*(byte *)(unaff_A6[3] + 0x8d) & 2) == 0) {
    unaff_A6[0x21] = 0x2251a;
    *(undefined2 *)(unaff_A6 + 0x22) = 0;
    *(undefined2 *)(unaff_A6 + 0x25) = 0;
  }
  else {
    unaff_A6[0x21] = 0x225de;
    *(undefined2 *)(unaff_A6 + 0x22) = 0;
    *(undefined2 *)(unaff_A6 + 0x25) = 0;
  }
  unaff_A6[0x1f] = 0xc00;
  unaff_A6[0x20] = 0;
  *(undefined2 *)(unaff_A6 + 0x25) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x96) = 0;
  *unaff_A6 = 0x22fbc;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_234de();
  if ((bool)uVar1) {
    *unaff_A6 = 0x22d08;
  }
  *(undefined2 *)(unaff_A6[3] + 0x9a) = *(undefined2 *)((int)unaff_A6 + 0x9a);
  unaff_A6[0xc] = unaff_A6[0xc] - unaff_A6[0x1f];
  if ((int)unaff_A6[0xc] < 0) {
    unaff_A6[0xc] = 0;
  }
  *(undefined4 *)(unaff_A5 + 0x694e) = unaff_A6[0xc];
  *(undefined4 *)(unaff_A5 + 0x6952) = unaff_A6[0xd];
  candidate_234f6();
  return;
}
