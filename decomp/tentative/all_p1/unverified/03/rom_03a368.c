/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a368.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3a368(void)

{
  undefined4 *unaff_A6;

  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x75) = 0xff;
  *unaff_A6 = 0x3a382;
  candidate_143dc();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_3a3a2;
  }
  if (*(char *)((int)unaff_A6 + 0x75) != '\0') {
    candidate_49e8();
  }
  candidate_4c24();
  return;
}
