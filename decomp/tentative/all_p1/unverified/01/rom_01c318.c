/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01c318.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1c318(void)

{
  byte bVar1;
  int unaff_A6;

  bVar1 = *(byte *)(unaff_A6 + 0x5f);
  *(byte *)(unaff_A6 + 0x5f) = bVar1 | 8;
  if ((bVar1 & 8) == 0) {
    *(undefined2 *)(unaff_A6 + 0x74) = 0x1e;
    candidate_c0d48();
  }
  return;
}
