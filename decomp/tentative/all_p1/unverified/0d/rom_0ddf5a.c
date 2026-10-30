/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ddf5a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ddf5a(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  unaff_A6[0xd] = 0x100000;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xfffe6667;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    unaff_A6[0xc] = 0xfffc0000;
  }
  else {
    unaff_A6[0xc] = 0x40000;
  }
  *(undefined4 *)((int)unaff_A6 + 0x7a) = unaff_A6[0xc];
  candidate_4256();
  *unaff_A6 = 0xddfb0;
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
  *(undefined4 *)((int)unaff_A6 + 0x7e) = unaff_A6[0xd];
  func_0x000143e4();
  unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x7a);
  unaff_A6[0xd] = *(undefined4 *)((int)unaff_A6 + 0x7e);
  if ((*(short *)(unaff_A6 + 0xd) < 0) && ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) {
    *unaff_A6 = 0xddcf4;
  }
  uVar1 = 0;
  candidate_427a();
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x76) + 0x5f) & 0x80) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  }
  candidate_1aeda();
  if (!(bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  }
  if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) &&
     (uVar1 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0')) {
    *unaff_A6 = candidate_de064;
    uVar1 = 0;
  }
  candidate_19504();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_de06e;
  }
  return;
}
