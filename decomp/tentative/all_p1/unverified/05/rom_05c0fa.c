/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05c0fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5c0fa(void)

{
  byte bVar1;
  byte bVar2;
  int unaff_A6;

  bVar1 = (byte)(((uint)*(ushort *)(unaff_A6 + 0xaa) << 5) >> 0x10);
  if (*(char *)(unaff_A6 + 0x7e) != '\0') {
    bVar2 = *(byte *)(unaff_A6 + 0x7e) & 3;
    if ((*(byte *)(unaff_A6 + 0x7e) & 4) != 0) {
      bVar2 = -bVar2;
    }
    bVar1 = bVar2 + bVar1;
    *(char *)(unaff_A6 + 0x7e) = *(char *)(unaff_A6 + 0x7e) + -1;
  }
  *(byte *)(unaff_A6 + 0x7f) = bVar1 & 0x1f;
  return;
}
