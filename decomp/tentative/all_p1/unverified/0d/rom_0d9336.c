/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d9336.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d9336(void)

{
  undefined1 uVar1;
  short sVar2;
  int unaff_A6;

  uVar1 = *(undefined1 *)(unaff_A6 + 0x84);
  sVar2 = (*(byte *)(unaff_A6 + 0x89) & 7) + 0x84;
  if (*(char *)(*(int *)(unaff_A6 + 0xc) + (int)sVar2) == *(char *)(unaff_A6 + 0x85)) {
    *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + (int)sVar2) = uVar1;
    *(undefined1 *)(unaff_A6 + 0x85) = uVar1;
  }
  return;
}
