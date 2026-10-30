/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d7f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_8d7f4(void)

{
  int iVar1;
  short in_D0w;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0x34) + *(int *)(unaff_A6 + 0x74 + (int)(short)(in_D0w << 2));
  *(int *)(unaff_A6 + 0x74 + (int)(short)(in_D0w << 2)) = iVar1;
  *(int *)(unaff_A6 + 0x2c) = iVar1;
  candidate_1abc8();
  return in_D0w;
}
