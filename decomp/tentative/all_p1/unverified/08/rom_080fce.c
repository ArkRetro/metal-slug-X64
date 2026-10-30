/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x080fce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x00080ff6: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x00080ffa) */
/* WARNING: Removing unreachable block (ram,0x00081006) */
/* WARNING: Removing unreachable block (ram,0x00080ffe) */

void candidate_80fce(void)

{
  ushort uVar1;
  ushort uVar2;
  char cVar3;
  short sVar4;
  ushort uVar5;
  byte unaff_D6b;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;
  bool bVar6;
  short sVar7;
  int iVar8;

  bVar6 = false;
  candidate_1aeb0();
  if (bVar6) {
    iVar8 = extraout_A0;
    cVar3 = candidate_1b14a();
    if ((byte)(cVar3 + 100U) >> 3 == *(byte *)(unaff_A6 + 0x91)) {
      bVar6 = false;
      sVar7 = 8;
      candidate_1ae00(8,iVar8,0x80ffa,iVar8);
      if (bVar6) {
        sVar4 = 0x18;
      }
      else {
        sVar4 = 0x20;
      }
      uVar1 = (*(short *)(iVar8 + 0x3c) + sVar7) - *(short *)(unaff_A6 + 0x3c);
      uVar2 = (*(short *)(iVar8 + 0x40) + sVar4) - *(short *)(unaff_A6 + 0x40);
      uVar5 = uVar1;
      if ((short)uVar1 < 0) {
        uVar5 = -uVar1;
      }
      if ((uVar5 & 0xff00) == 0) {
        uVar5 = uVar2;
        if ((short)uVar2 < 0) {
          uVar5 = -uVar2;
        }
        if ((uVar5 & 0xff00) == 0) {
          *(ushort *)(unaff_A6 + 0x30) = uVar1;
          *(ushort *)(unaff_A6 + 0x34) = uVar2;
          candidate_128da();
          *(undefined1 *)(unaff_A5 + 0x4009) = 0;
          *(undefined1 *)(unaff_A5 + 0x4008) = 1;
          candidate_13968();
          *(undefined4 *)(unaff_A6 + 0x30) = 0;
          *(undefined4 *)(unaff_A6 + 0x34) = 0;
          if ((unaff_D6b & 2) == 0) {
            return;
          }
        }
      }
    }
  }
  return;
}
