/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x012938.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_12938(void)

{
  ushort unaff_D6w;
  byte bVar1;
  uint unaff_D7;
  ushort uVar2;
  int in_A0;
  int unaff_A5;

  bVar1 = (byte)unaff_D6w;
  if (in_A0 == 0) {
    return;
  }
  if ((in_A0 == 0x10c72e) || (in_A0 == 0x10d7b6)) {
    candidate_c79fe();
    uVar2 = (unaff_D6w & 0xe0) >> 5;
    if (*(int *)(unaff_A5 + 0x400e) != 0) {
      uVar2 = (ushort)*(byte *)(*(int *)(unaff_A5 + 0x400e) + (int)(short)uVar2);
    }
    if (uVar2 == 0xb) {
      unaff_D7 = (uint)(ushort)((unaff_D6w & 0xf) + 0x10);
    }
    else {
      unaff_D7 = CONCAT22(uVar2,unaff_D6w) & 0xffff000f;
    }
    bVar1 = bVar1 & 0x10;
  }
  else {
    candidate_1431a();
  }
  uVar2 = (ushort)unaff_D7;
  if (((short)(unaff_D7 >> 0x10) == 1) && (*(char *)(unaff_A5 + 0x400d) != '\0')) {
    bVar1 = 0;
    uVar2 = 0;
  }
  if ((bVar1 != 0) &&
     (*(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 8,
     *(char *)(unaff_A5 + 0x4008) == '\0')) {
    return;
  }
  if (0x1d < uVar2) {
    return;
  }
  return;
}
