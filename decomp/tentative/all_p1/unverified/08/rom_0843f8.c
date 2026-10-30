/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0843f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_843f8(void)

{
  int unaff_A5;
  int unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0x8820;
  candidate_405ea();
  *(undefined1 *)(unaff_A6 + 0x5b) = *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x5b);
  if ((*(ushort *)(unaff_A5 + -0x7ff2) & 1) == 0) {
    *(undefined1 *)(unaff_A6 + 0x5a) = 5;
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x40;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x5a) = 7;
  }
  candidate_406b6();
  return;
}
