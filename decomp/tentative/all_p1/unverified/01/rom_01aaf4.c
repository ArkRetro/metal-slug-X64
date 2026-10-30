/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01aaf4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_1aaf4(void)

{
  short sVar1;
  short in_D0w;
  ushort uVar2;
  int unaff_A5;

  if (*(char *)(unaff_A5 + 0x4338) == '\0') {
    sVar1 = *(short *)(unaff_A5 + 0x4334);
    uVar2 = in_D0w + sVar1;
    if (*(ushort *)(unaff_A5 + 0x4336) <= uVar2) {
      uVar2 = *(ushort *)(unaff_A5 + 0x4336);
    }
    *(ushort *)(unaff_A5 + 0x4334) = uVar2;
    return sVar1 + (in_D0w - uVar2);
  }
  sVar1 = *(short *)(unaff_A5 + 0x4336);
  uVar2 = sVar1 - in_D0w;
  if (uVar2 < *(ushort *)(unaff_A5 + 0x4334)) {
    uVar2 = *(ushort *)(unaff_A5 + 0x4334);
  }
  *(ushort *)(unaff_A5 + 0x4336) = uVar2;
  return uVar2 + (in_D0w - sVar1);
}
