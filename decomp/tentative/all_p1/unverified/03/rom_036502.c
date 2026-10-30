/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x036502.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_36502(void)

{
  int iVar1;
  int unaff_A6;
  longlong lVar2;

  *(undefined4 *)(unaff_A6 + 0x30) = 0;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  iVar1 = *(int *)(*(int *)(unaff_A6 + 0xc) + 0x82);
  if (iVar1 != 0) {
    func_0x000366b6();
    lVar2 = candidate_2e7c();
    iVar1 = (int)((ulonglong)lVar2 >> 0x20);
    *(longlong *)(unaff_A6 + 0x30) = lVar2;
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
    if (-1 < lVar2) {
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
    }
  }
  return iVar1;
}
