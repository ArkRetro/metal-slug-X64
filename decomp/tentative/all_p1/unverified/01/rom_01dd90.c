/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01dd90.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_1dd90(void)

{
  int iVar1;
  int unaff_A6;

  candidate_1abc8();
  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined1 *)(unaff_A6 + 0x12) = *(undefined1 *)(iVar1 + 0x12);
  *(undefined1 *)(unaff_A6 + 0x13) = *(undefined1 *)(iVar1 + 0x13);
  if ((*(byte *)(iVar1 + 0x15) & 1) == 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0x7f;
  if ((*(char *)(unaff_A6 + 0x94) == '\0') && ((*(byte *)(iVar1 + 0x5f) & 0x80) != 0)) {
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x80;
  }
  return *(undefined2 *)(iVar1 + 0x4e);
}
