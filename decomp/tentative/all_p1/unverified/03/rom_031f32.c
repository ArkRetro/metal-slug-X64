/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x031f32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_31f32(void)

{
  int iVar1;
  byte bVar2;
  byte *pbVar3;
  byte *pbVar4;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0x7a);
  pbVar3 = *(byte **)(iVar1 + 0x76) + 1;
  bVar2 = **(byte **)(iVar1 + 0x76);
  if ((char)bVar2 < '\0') {
    pbVar4 = pbVar3 + -(int)(short)(ushort)*pbVar3;
    pbVar3 = pbVar4 + 1;
    bVar2 = *pbVar4;
  }
  *(byte **)(iVar1 + 0x76) = pbVar3;
  *(byte *)(iVar1 + 0x80) = bVar2;
  return;
}
