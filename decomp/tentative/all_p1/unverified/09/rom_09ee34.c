/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09ee34.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9ee34(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_9ea8();
  unaff_A6[0xd] = 0x66666;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff5c29;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    unaff_A6[0xc] = 0xfffe6667;
  }
  else {
    unaff_A6[0xc] = 0x19999;
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  candidate_4256();
  *unaff_A6 = 0x9ee8c;
  func_0x000143e4();
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_41ac;
  }
  *(short *)((int)unaff_A6 + 0x26) = *(short *)((int)unaff_A6 + 0x26) + -4;
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_9ec34;
  }
  bVar1 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
