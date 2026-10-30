/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d1e2c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d1e2c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
  unaff_A6[0xc] = (int)((uint)*(ushort *)((int)unaff_A6 + 0x8a) << 0x10) >> 8;
  *unaff_A6 = 0xd1e44;
  uVar1 = false;
  candidate_d3314();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d1e70;
    uVar1 = 0;
  }
  func_0x000143e4();
  candidate_d3256();
  candidate_d31be();
  candidate_427a();
  candidate_d338a();
  candidate_d3280();
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d1f2e;
  }
  return;
}
