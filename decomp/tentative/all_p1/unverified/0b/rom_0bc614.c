/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bc614.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bc614(void)

{
  ushort uVar1;
  byte bVar2;
  int unaff_A6;

  *(short *)(unaff_A6 + 0x6e) = *(short *)(unaff_A6 + 0x6e) - (*(ushort *)(unaff_A6 + 0x6e) >> 4);
  uVar1 = candidate_29c6();
  *(short *)(unaff_A6 + 0x6e) = *(short *)(unaff_A6 + 0x6e) - ((uVar1 & 0x7f) + 0x20);
  bVar2 = *(byte *)(unaff_A6 + 0x6e);
  if ((char)bVar2 < '\0') {
    bVar2 = ~bVar2;
  }
  *(byte *)(unaff_A6 + 0x70) = bVar2 >> 2 & 0x1f;
  func_0x00009cce();
  func_0x00009cce();
  func_0x00009cce();
  return;
}
