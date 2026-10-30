/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x013c1a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x00013c76) */
/* WARNING: Removing unreachable block (ram,0x00013c9e) */
/* WARNING: Removing unreachable block (ram,0x00013ca8) */
/* WARNING: Removing unreachable block (ram,0x00013cb0) */
/* WARNING: Removing unreachable block (ram,0x00013cc2) */
/* WARNING: Removing unreachable block (ram,0x00013cc8) */
/* WARNING: Removing unreachable block (ram,0x00013cd0) */

void candidate_13c1a(void)

{
  int extraout_A1;
  int iVar1;
  int extraout_A1_00;
  int unaff_A5;

  *(undefined4 *)(unaff_A5 + 0x4060) = 0;
  *(undefined4 *)(unaff_A5 + 0x405c) = 0x80000000;
  *(undefined4 *)(unaff_A5 + 0x4064) = 0x4000;
  candidate_1381c();
  candidate_1382a();
  iVar1 = extraout_A1;
  while (iVar1 != 0) {
    candidate_12938();
    *(undefined2 *)(unaff_A5 + 0x4018) = 0;
    candidate_12938();
    candidate_1382a();
    iVar1 = extraout_A1_00;
  }
  candidate_12e64();
  return;
}
