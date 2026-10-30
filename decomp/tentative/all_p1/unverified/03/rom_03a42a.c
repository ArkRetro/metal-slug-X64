/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a42a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3a42a(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x72) = 2;
  *unaff_A6 = 0x3a436;
  candidate_1436a();
  candidate_427a();
  sVar1 = *(short *)((int)unaff_A6 + 0x72);
  *(short *)((int)unaff_A6 + 0x72) = sVar1 + -1;
  if (sVar1 == 0) {
    *(undefined1 *)(unaff_A6[3] + 0x74) = 1;
    candidate_b7a2c();
    *unaff_A6 = 0x3a5c2;
  }
  if (*(char *)((int)unaff_A6 + 0x75) != '\0') {
    candidate_49e8();
  }
  candidate_4c24();
  return;
}
