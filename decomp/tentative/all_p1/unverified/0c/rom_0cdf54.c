/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cdf54.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cdf54(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  unaff_A6[0x14] = 0;
  unaff_A6[0xc] = 0xfff80000;
  *(undefined2 *)((int)unaff_A6 + 0x9e) = 0x40;
  unaff_A6[0xd] = 0x20000;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0xffc0;
  candidate_9ea8();
  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  candidate_4256();
  *unaff_A6 = 0xcdfce;
  unaff_A6[0xc] = ((int)((uint)*(ushort *)((int)unaff_A6 + 0x9e) << 0x10) >> 8) + unaff_A6[0xc];
  unaff_A6[0xd] = ((int)((uint)*(ushort *)(unaff_A6 + 0x1c) << 0x10) >> 8) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_ce026;
  }
  candidate_427a();
  if (-0x8001 < (int)unaff_A6[0xc]) {
    *(undefined2 *)((int)unaff_A6 + 0x9e) = 0;
  }
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    *unaff_A6 = candidate_ce098;
  }
  return;
}
