/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d5a92.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_d5a92(void)

{
  int iVar1;
  int unaff_A5;
  undefined4 *unaff_A6;

  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    candidate_140c0();
  }
  candidate_1abc8();
  candidate_427a();
  if (((*(byte *)((int)unaff_A6 + 0x76) ^ *(byte *)(unaff_A5 + 0x4606)) & 1) != 0) {
    *unaff_A6 = candidate_d5ad4;
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  iVar1 = *(int *)((int)unaff_A6 + 0x96);
  if (iVar1 != 0) {
    iVar1 = candidate_49e8();
  }
  return iVar1;
}
