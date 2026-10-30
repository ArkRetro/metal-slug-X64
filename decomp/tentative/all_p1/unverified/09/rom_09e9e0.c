/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09e9e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e9e0(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  unaff_A6[0xd] = 0x2aaaa;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff1c72;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    unaff_A6[0xc] = 0x20000;
  }
  else {
    unaff_A6[0xc] = 0xfffe0000;
  }
  if ((*(byte *)((int)unaff_A6 + 0x96) & 1) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x9ea42;
  func_0x000143e4();
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_9ea6a;
  }
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
