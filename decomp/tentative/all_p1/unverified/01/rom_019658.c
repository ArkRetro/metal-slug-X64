/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x019658.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_19658(void)

{
  int iVar1;
  int unaff_A5;

  iVar1 = *(int *)(unaff_A5 + 0x431e);
  if (iVar1 != 0) {
    for (; *(ushort *)(iVar1 + 0xc) <= *(ushort *)(unaff_A5 + 0x699e); iVar1 = iVar1 + 0xc) {
    }
    *(int *)(unaff_A5 + 0x431e) = iVar1;
    if (*(char *)(unaff_A5 + 0x4323) == '\0') {
      *(int *)(unaff_A5 + 0x431a) = iVar1 + 2;
    }
  }
  if ((DAT_0010f001 & 2) != 0) {
    candidate_a03e();
  }
  return;
}
