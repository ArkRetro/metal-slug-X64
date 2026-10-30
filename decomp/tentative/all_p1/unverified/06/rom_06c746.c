/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06c746.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6c746(void)

{
  char cVar1;
  byte bVar2;
  int unaff_A6;

  if (((short)((short)((uint)*(undefined4 *)(unaff_A6 + 0x30) >> 8) >> 4) == 0) &&
     ((short)((short)((uint)*(undefined4 *)(unaff_A6 + 0x34) >> 8) >> 4) == 0)) {
    cVar1 = *(char *)(unaff_A6 + 0x94);
  }
  else {
    cVar1 = candidate_2eb4();
  }
  *(char *)(unaff_A6 + 0x94) =
       ((char)(cVar1 - *(char *)(unaff_A6 + 0x94)) >> 1) + *(char *)(unaff_A6 + 0x94);
  bVar2 = (byte)(*(char *)(unaff_A6 + 0x94) - 0x3cU) >> 3;
  if (0x10 < bVar2) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
    bVar2 = -(bVar2 - 0x20);
  }
  *(byte *)(unaff_A6 + 0x95) = bVar2;
  candidate_427a();
  return;
}
