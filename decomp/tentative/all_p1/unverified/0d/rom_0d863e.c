/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d863e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d863e(void)

{
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  unaff_A6[0xc] = 0xfffc0000;
  *(undefined2 *)((int)unaff_A6 + 0x96) = 0x20;
  unaff_A6[0xd] = 0x10000;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0xffd8;
  candidate_9ea8();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_4256();
  *unaff_A6 = 0xd868e;
  unaff_A6[0xc] = ((int)((uint)*(ushort *)((int)unaff_A6 + 0x96) << 0x10) >> 8) + unaff_A6[0xc];
  unaff_A6[0xd] = ((int)((uint)*(ushort *)(unaff_A6 + 0x1c) << 0x10) >> 8) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_d86e6;
  }
  candidate_427a();
  if (-0x8001 < (int)unaff_A6[0xc]) {
    *(undefined2 *)((int)unaff_A6 + 0x96) = 0;
  }
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    *unaff_A6 = candidate_d87c6;
  }
  return;
}
