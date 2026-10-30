/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03dd96.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 candidate_3dd96(void)

{
  byte bVar1;
  undefined2 uVar2;
  short sVar3;
  short in_D0w;
  undefined4 uVar4;
  ushort uVar6;
  uint uVar5;
  undefined4 in_D1;
  byte bVar8;
  ushort uVar7;
  byte unaff_D2b;
  char cVar9;
  char cVar10;
  byte *in_A0;
  int iVar11;
  undefined4 *puVar12;
  short *psVar13;

  bVar8 = (byte)in_D1;
  uVar7 = (ushort)in_D1;
  if (bVar8 < 0x10) {
    uVar2 = (undefined2)
            CONCAT31((int3)(CONCAT22(in_D0w,(short)CONCAT31((int3)(CONCAT22(in_D0w,uVar7) >> 8),
                                                            bVar8 << 4) << 4) >> 8),unaff_D2b);
    _DAT_003c0000 = CONCAT22(0x8200,uVar2);
    return CONCAT44(CONCAT22(uVar2,in_D0w + 0x20),in_D1);
  }
  if (*in_A0 == 0xde) {
    if (unaff_D2b == 0xb3) {
      in_A0 = in_A0 + 1;
      unaff_D2b = 0xe0;
    }
    else if ((unaff_D2b < 0xb6) || (0xc4 < unaff_D2b)) {
      if ((unaff_D2b < 0xca) || (0xce < unaff_D2b)) goto LAB_0003ddfe;
      in_A0 = in_A0 + 1;
      unaff_D2b = (unaff_D2b + 0x36) * '\x02' - 0x10;
    }
    else {
      in_A0 = in_A0 + 1;
      unaff_D2b = unaff_D2b + 0x2b;
    }
  }
  else {
LAB_0003ddfe:
    if (((*in_A0 == 0xdf) && (0xc9 < unaff_D2b)) && (unaff_D2b < 0xcf)) {
      in_A0 = in_A0 + 1;
      unaff_D2b = (unaff_D2b + 0x36) * '\x02' - 0xf;
    }
  }
  if (bVar8 == 0x30) {
    uVar4 = candidate_3cf9a();
    goto LAB_0003e18e;
  }
  if (bVar8 == 0x61) {
    _DAT_003c0000 =
         CONCAT31((int3)(CONCAT22(in_D0w,(short)CONCAT31((int3)(CONCAT22(in_D0w,uVar7) >> 8),0xa0)
                                         << 4) >> 8),(unaff_D2b & 0xf) + (unaff_D2b & 0xf0) * '\x02'
                 ) + 0x10010;
    uVar4 = CONCAT22((short)_DAT_003c0000,(short)(_DAT_003c0000 >> 0x10) + 0x1f);
    goto LAB_0003e18e;
  }
  if (bVar8 == 0x48) {
    iVar11 = 0x3d596;
LAB_0003de94:
    iVar11 = CONCAT22(in_D0w,*(ushort *)(iVar11 + (short)((ushort)unaff_D2b * 2)) |
                             (uVar7 & 0xf00) << 4);
  }
  else {
    if (bVar8 == 0x4b) {
      iVar11 = 0x3d796;
      goto LAB_0003de94;
    }
    if (bVar8 != 0x41) {
      if (bVar8 == 0x31) {
        _DAT_003c0000 =
             CONCAT31((int3)(CONCAT22(in_D0w,(short)CONCAT31((int3)(CONCAT22(in_D0w,uVar7) >> 8),
                                                             0x40) << 4) >> 8),unaff_D2b + 0x80) +
             0x20020;
        uVar4 = CONCAT22((short)_DAT_003c0000,(short)(_DAT_003c0000 >> 0x10) + 0x1e);
        goto LAB_0003e18e;
      }
      if (bVar8 == 0x68) {
        iVar11 = 0x3db96;
LAB_0003df6e:
        if (unaff_D2b == 0x40) {
          unaff_D2b = *in_A0;
          cVar9 = '\0';
          if ((((unaff_D2b == 0x2e) || (cVar9 = '\x01', unaff_D2b == 0x3a)) ||
              (cVar9 = '\x02', unaff_D2b == 0x7e)) || (unaff_D2b == 0x2d)) {
            unaff_D2b = in_A0[1];
            cVar10 = cVar9;
            if (((unaff_D2b == 0x61) || (cVar10 = cVar9 + '\x03', unaff_D2b == 0x75)) ||
               ((cVar10 = cVar9 + '\x06', unaff_D2b == 0x65 ||
                (cVar10 = cVar9 + '\b', unaff_D2b == 0x6f)))) {
              unaff_D2b = cVar10 + 0x80;
            }
          }
          else if (unaff_D2b == 0x60) {
            unaff_D2b = in_A0[1];
          }
        }
        uVar6 = *(ushort *)(iVar11 + (short)((ushort)unaff_D2b * 2));
      }
      else {
        if (bVar8 == 0x6b) {
          iVar11 = 0x3d996;
          goto LAB_0003df6e;
        }
        if (bVar8 == 0x58) {
          if ((unaff_D2b < 0x80) || ((0x9f < unaff_D2b && (unaff_D2b < 0xe0)))) goto LAB_0003deb4;
          puVar12 = (undefined4 *)0x3d512;
          do {
            uVar4 = *puVar12;
            sVar3 = (short)uVar4;
            if (CONCAT11(unaff_D2b,*in_A0) == sVar3) break;
            puVar12 = puVar12 + 1;
          } while (sVar3 != 0);
          iVar11 = CONCAT22(in_D0w,(uVar7 & 0xf00) << 4 | (ushort)((uint)uVar4 >> 0x10));
          goto LAB_0003dede;
        }
        if ((bVar8 != 0x32) || (unaff_D2b != 0x40)) {
          uVar4 = candidate_3cf9a();
          goto LAB_0003e18e;
        }
        bVar8 = *in_A0;
        if (bVar8 != 0x21) {
          if ((bVar8 == 99) || (bVar8 == 0x43)) {
            uVar5 = CONCAT31((int3)(CONCAT22(in_D0w,(short)CONCAT31((int3)(CONCAT22(in_D0w,uVar7) >>
                                                                          8),0x30) << 4) >> 8),bVar8
                            ) + 0x10000U & 0xffff0f00;
            _DAT_003c0000 = uVar5 | 0xbeb;
            uVar4 = CONCAT22((short)_DAT_003c0000,(short)(uVar5 >> 0x10) + 0x1f);
          }
          else {
            bVar1 = in_A0[1];
            psVar13 = (short *)0x3e194;
            if (((((bVar1 == 0x61) || (psVar13 = (short *)0x3e1ce, bVar1 == 0x69)) ||
                 (psVar13 = (short *)0x3e1d8, bVar1 == 0x75)) ||
                ((psVar13 = (short *)0x3e1f2, bVar1 == 0x65 ||
                 (psVar13 = (short *)0x3e214, bVar1 == 0x6f)))) ||
               ((((psVar13 = (short *)0x3e23e, bVar1 == 0x41 ||
                  ((psVar13 = (short *)0x3e260, bVar1 == 0x49 ||
                   (psVar13 = (short *)0x3e26a, bVar1 == 0x55)))) ||
                 (psVar13 = (short *)0x3e274, bVar1 == 0x45)) ||
                (psVar13 = (short *)0x3e28e, bVar1 == 0x4f)))) {
              while( true ) {
                if (*psVar13 == 0) break;
                if (*psVar13 == CONCAT11(bVar8,bVar1)) {
                  uVar5 = CONCAT22(in_D0w,psVar13[1] | (uVar7 & 0xf00) << 4) - 0x10000U & 0xfffff000
                  ;
                  uVar7 = psVar13[2] | (ushort)uVar5;
                  sVar3 = (short)(uVar5 >> 0x10);
                  _DAT_003c0000 = CONCAT22(sVar3,uVar7);
                  uVar4 = CONCAT22(uVar7,sVar3 + 0x21);
                  goto LAB_0003e18e;
                }
                psVar13 = psVar13 + 4;
              }
            }
            _DAT_003c0000 =
                 CONCAT31((int3)(CONCAT22(in_D0w,(short)CONCAT31((int3)(CONCAT22(in_D0w,uVar7) >> 8)
                                                                 ,0x30) << 4) >> 8),bVar1);
            uVar4 = CONCAT22((short)_DAT_003c0000,in_D0w + 0x20);
          }
          goto LAB_0003e18e;
        }
        uVar6 = 0xbea;
      }
      uVar6 = (uVar7 & 0xf00) << 4 | uVar6;
      _DAT_003c0000 = CONCAT22(in_D0w,uVar6);
      uVar4 = CONCAT22(uVar6,in_D0w + 0x20);
      goto LAB_0003e18e;
    }
LAB_0003deb4:
    uVar4 = CONCAT31((int3)(CONCAT22(in_D0w,(short)CONCAT31((int3)(CONCAT22(in_D0w,uVar7) >> 8),0x80
                                                           ) << 4) >> 8),(unaff_D2b & 0xf0) * '\x02'
                    );
    if (7 < (unaff_D2b & 0xf)) {
      uVar4 = CONCAT22(in_D0w,(short)uVar4 + 0x100);
    }
    iVar11 = CONCAT31((int3)((uint)uVar4 >> 8),(unaff_D2b & 7) * '\x02' | (byte)uVar4);
  }
LAB_0003dede:
  _DAT_003c0000 = iVar11 + 0x200001;
  uVar4 = CONCAT22((short)_DAT_003c0000,(short)(_DAT_003c0000 >> 0x10) + 0x20);
LAB_0003e18e:
  return CONCAT44(uVar4,in_D1);
}
