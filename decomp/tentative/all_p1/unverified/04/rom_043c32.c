/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043c32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_43c32(void)

{
  ushort uVar1;
  int unaff_A5;
  int unaff_A6;

  if ((*(ushort *)(unaff_A5 + -0x7ff2) & 0xf) == (*(byte *)(unaff_A6 + 0xa0) & 0xf)) {
    candidate_43c74();
  }
  candidate_43d22();
  candidate_446d4();
  uVar1 = candidate_29c6();
  uVar1 = uVar1 & 0x1f;
  if (uVar1 == 0) {
    uVar1 = candidate_43daa();
  }
  return uVar1;
}
