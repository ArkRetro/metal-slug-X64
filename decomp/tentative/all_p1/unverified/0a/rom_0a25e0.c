/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a25e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a25e0(void)

{
  short sVar1;
  byte bVar2;
  char cVar3;
  int iVar4;
  int unaff_A6;
  bool bVar5;

  *(undefined1 *)(unaff_A6 + 0x17) = *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x17);
  bVar2 = *(byte *)(*(int *)(unaff_A6 + 0xc) + 0x87);
  *(byte *)(unaff_A6 + 0x87) = bVar2;
  sVar1 = *(short *)((short)((ushort)bVar2 * 2) + 0x29fc);
  cVar3 = *(char *)(*(int *)(unaff_A6 + 0xc) + 0x8d);
  *(char *)(unaff_A6 + 0x8d) = cVar3;
  if (cVar3 == '\0') {
    iVar4 = 7;
  }
  else {
    iVar4 = 6;
  }
  sVar1 = (short)(sVar1 + -8) >> iVar4;
  *(short *)(unaff_A6 + 0x40) = sVar1 + *(short *)(unaff_A6 + 0x40);
  bVar5 = false;
  candidate_427a();
  if (bVar5) {
    *(undefined1 *)(unaff_A6 + 0x61) = 0xff;
  }
  *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) - sVar1;
  return;
}
