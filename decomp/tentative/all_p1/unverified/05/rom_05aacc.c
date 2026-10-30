/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05aacc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5aacc(void)

{
  undefined4 *unaff_A6;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
  *unaff_A6 = 0x5aaf0;
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_5ab12;
  }
  candidate_427a();
  candidate_5b714();
  return;
}
