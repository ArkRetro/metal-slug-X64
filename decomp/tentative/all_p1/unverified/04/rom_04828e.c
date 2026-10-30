/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04828e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4828e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  candidate_4256();
  *unaff_A6 = 0x482ba;
  uVar1 = 0;
  candidate_49e9e();
  func_0x000143e4();
  candidate_427a();
  if ((bool)uVar1) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    *unaff_A6 = candidate_482ee;
    uVar1 = 0;
  }
  candidate_49ece();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_4783a;
  }
  candidate_4900c();
  return;
}
