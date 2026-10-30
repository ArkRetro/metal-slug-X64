/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d50cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d50cc(void)

{
  int iVar1;
  int unaff_A6;

  if (((*(byte *)(unaff_A6 + 0x7b) & 1) != 0) && (iVar1 = *(int *)(unaff_A6 + 0xc), iVar1 != 0)) {
    *(char *)(iVar1 + 0x61) = *(char *)(iVar1 + 0x61) + '\x01';
    *(undefined1 *)(unaff_A6 + 0x60) = *(undefined1 *)(iVar1 + 0x60);
  }
  return;
}
