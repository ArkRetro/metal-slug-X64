/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aee0c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ulonglong candidate_aee0c(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  ushort uVar4;
  undefined4 uVar5;
  undefined2 extraout_D1u;
  undefined2 extraout_D1u_00;
  uint in_D1;
  uint extraout_D1;
  short sVar7;
  short sVar8;
  int unaff_A6;
  bool bVar9;
  int iVar6;

  if (((((0x17 < *(short *)(unaff_A6 + 0x3c)) && (*(short *)(unaff_A6 + 0x3c) < 0x129)) &&
       ((*(byte *)(unaff_A6 + 0x7c) & 8) != 0)) &&
      ((bVar9 = false, -1 < *(char *)(unaff_A6 + 0x80) &&
       (candidate_aedfa(), in_D1 = extraout_D1, bVar9)))) && (*(short *)(unaff_A6 + 0x70) == 0)) {
    bVar1 = *(byte *)(unaff_A6 + 0x81);
    if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
      bVar1 = bVar1 & 2;
    }
    in_D1 = CONCAT31((int3)(extraout_D1 >> 8),bVar1);
    if ((((*(byte *)(unaff_A6 + 0x7c) & 0x40) != 0) &&
        (in_D1 = in_D1 & 0xfffffffd, (bVar1 & 2) != 0)) && ((*(byte *)(unaff_A6 + 0x7c) & 2) != 0))
    {
      candidate_aeb40();
      in_D1 = CONCAT22(extraout_D1u,(short)in_D1);
    }
    if ((char)in_D1 != '\0') {
      uVar5 = candidate_29f2();
      bVar1 = *(byte *)(unaff_A6 + 0x81);
      uVar4 = (ushort)((uint)uVar5 >> 0x10);
      iVar6 = CONCAT22(uVar4,7);
      bVar2 = bVar1 * '\x02';
      sVar7 = (short)uVar5 - (ushort)CARRY1(bVar1,bVar1);
      in_D1 = CONCAT22(extraout_D1u_00,sVar7);
      if (!CARRY1(bVar1,bVar1) || (short)uVar5 != 0) {
        iVar6 = CONCAT22(uVar4,6);
        bVar3 = bVar1 * '\x04';
        sVar8 = sVar7 - (ushort)CARRY1(bVar2,bVar2);
        in_D1 = CONCAT22(extraout_D1u_00,sVar8);
        if (!CARRY1(bVar2,bVar2) || sVar7 != 0) {
          iVar6 = CONCAT22(uVar4,5);
          bVar2 = bVar1 * '\b';
          sVar7 = sVar8 - (ushort)CARRY1(bVar3,bVar3);
          in_D1 = CONCAT22(extraout_D1u_00,sVar7);
          if (!CARRY1(bVar3,bVar3) || sVar8 != 0) {
            iVar6 = CONCAT22(uVar4,4);
            bVar3 = bVar1 * '\x10';
            sVar8 = sVar7 - (ushort)CARRY1(bVar2,bVar2);
            in_D1 = CONCAT22(extraout_D1u_00,sVar8);
            if (!CARRY1(bVar2,bVar2) || sVar7 != 0) {
              iVar6 = CONCAT22(uVar4,3);
              bVar2 = bVar1 * ' ';
              sVar7 = sVar8 - (ushort)CARRY1(bVar3,bVar3);
              in_D1 = CONCAT22(extraout_D1u_00,sVar7);
              if (!CARRY1(bVar3,bVar3) || sVar8 != 0) {
                iVar6 = CONCAT22(uVar4,2);
                bVar3 = bVar1 * '@';
                sVar8 = sVar7 - (ushort)CARRY1(bVar2,bVar2);
                in_D1 = CONCAT22(extraout_D1u_00,sVar8);
                if (!CARRY1(bVar2,bVar2) || sVar7 != 0) {
                  iVar6 = CONCAT22(uVar4,1);
                  bVar1 = bVar1 * -0x80;
                  sVar7 = sVar8 - (ushort)CARRY1(bVar3,bVar3);
                  in_D1 = CONCAT22(extraout_D1u_00,sVar7);
                  if (!CARRY1(bVar3,bVar3) || sVar8 != 0) {
                    iVar6 = (uint)uVar4 << 0x10;
                    in_D1 = CONCAT22(extraout_D1u_00,sVar7 - (ushort)CARRY1(bVar1,bVar1));
                    if (!CARRY1(bVar1,bVar1) || sVar7 != 0) goto LAB_000aef0e;
                  }
                }
              }
            }
          }
        }
      }
      return CONCAT44(iVar6,in_D1);
    }
  }
LAB_000aef0e:
  return (ulonglong)in_D1;
}
