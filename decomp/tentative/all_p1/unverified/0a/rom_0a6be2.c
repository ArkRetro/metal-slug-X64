/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6be2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a6be2(void)

{
  ushort *puVar1;
  byte bVar2;
  short in_D0w;
  int unaff_A6;

  bVar2 = *(byte *)(*(int *)((short)(in_D0w * 4) + 0x2a6334) +
                   (int)(short)(ushort)*(byte *)(unaff_A6 + 0x7b));
  if ((char)bVar2 < '\0') {
    return;
  }
  puVar1 = (ushort *)(*(int *)(unaff_A6 + 0x6e) + 0x74);
  *puVar1 = 1 << (bVar2 & 0x3f) | *puVar1;
  *(char *)(unaff_A6 + 0x7b) = *(char *)(unaff_A6 + 0x7b) + '\x01';
  return;
}
