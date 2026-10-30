/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a48c2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a48c2(void)

{
  int iVar1;
  int unaff_A6;

  if (*(int *)(unaff_A6 + 0xa0) < 1) {
    *(byte *)(unaff_A6 + 0x60) = *(byte *)(unaff_A6 + 0x60) | 1;
    func_0x00009cce();
    func_0x00009cce();
  }
  else {
    iVar1 = *(int *)(unaff_A6 + 0xa0) - *(int *)(unaff_A6 + 0x8c);
    *(int *)(unaff_A6 + 0xa0) = iVar1;
    if (iVar1 < 0) {
      *(undefined4 *)(unaff_A6 + 0xa0) = 0;
    }
    func_0x00009cce();
    func_0x00009cce();
  }
  return;
}
