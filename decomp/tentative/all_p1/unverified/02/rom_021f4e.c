/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021f4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21f4e(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  candidate_4256();
  iVar1 = (int)*(short *)((int)unaff_A6 + 0x72) << 8;
  *(int *)((int)unaff_A6 + 0x62) = iVar1;
  if (iVar1 < 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *unaff_A6 = 0x21f84;
  candidate_1456a();
  candidate_427a();
  candidate_220bc();
  return;
}
