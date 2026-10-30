/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01b242.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1b242(void)

{
  int in_A0;
  int unaff_A6;

  *(undefined2 *)(in_A0 + 0x1e) = *(undefined2 *)(unaff_A6 + 0x1e);
  *(undefined2 *)(in_A0 + 0x66) = *(undefined2 *)(unaff_A6 + 0x66);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    *(byte *)(in_A0 + 0x15) = *(byte *)(in_A0 + 0x15) | 1;
  }
  if ((*(byte *)(unaff_A6 + 0x15) & 2) != 0) {
    *(byte *)(in_A0 + 0x15) = *(byte *)(in_A0 + 0x15) | 2;
  }
  if ((*(byte *)(unaff_A6 + 0x15) & 4) != 0) {
    *(byte *)(in_A0 + 0x15) = *(byte *)(in_A0 + 0x15) | 4;
  }
  return;
}
