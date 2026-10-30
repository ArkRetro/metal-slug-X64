/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0768ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_768ae(void)

{
  int iVar1;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined1 *)(unaff_A6 + 0x15) = *(undefined1 *)(iVar1 + 0x15);
  *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)(iVar1 + 0x1e);
  *(undefined1 *)(unaff_A6 + 0x12) = *(undefined1 *)(iVar1 + 0x12);
  *(undefined1 *)(unaff_A6 + 0x13) = *(undefined1 *)(iVar1 + 0x13);
  if ((*(byte *)(iVar1 + 0x84) & 1) == 0) {
    *(undefined1 *)(unaff_A6 + 0x70) = 0x10;
    if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      *(undefined1 *)(unaff_A6 + 0x70) = 0;
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x70) = *(undefined1 *)(iVar1 + 0x70);
  }
  return;
}
