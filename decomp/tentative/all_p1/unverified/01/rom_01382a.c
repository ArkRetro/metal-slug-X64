/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01382a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_1382a(void)

{
  int iVar1;
  undefined4 in_D0;
  int in_D1;
  int unaff_D3;
  int iVar2;
  short sVar3;
  int iVar4;
  short sVar5;
  char cVar6;
  short sVar7;
  int in_A0;
  int iVar8;
  int *piVar9;
  int unaff_A5;
  int unaff_A6;

  piVar9 = *(int **)(unaff_A5 + 0x406a);
  do {
    do {
      iVar8 = *piVar9;
      if (iVar8 == 0) {
        cVar6 = *(char *)(unaff_A5 + 0x42a2);
        if (cVar6 == '\0') {
          return CONCAT44(in_D0,in_D1);
        }
        sVar7 = *(short *)(unaff_A5 + 0x42a4);
        iVar8 = unaff_A5 + 0x406e + (int)(short)(sVar7 << 6);
        iVar1 = unaff_A6 + 0x28;
        while ((((((-1 < cVar6 || (*(char *)(unaff_A6 + 0x6a) == (char)sVar7)) ||
                  (*(short *)(iVar8 + 0x26) == 0)) ||
                 ((iVar4 = *(int *)(iVar8 + 0x10), iVar4 != 0 &&
                  ((iVar1 == iVar4 ||
                   ((iVar4 = *(int *)(iVar4 + 0x10), iVar4 != 0 &&
                    ((iVar1 == iVar4 ||
                     ((*(int *)(iVar4 + 0x10) != 0 && (iVar1 == *(int *)(iVar4 + 0x10)))))))))))))
                || (*(int *)(unaff_A5 + 0x429a) < *(int *)(iVar8 + 0x2c))) ||
               (((*(int *)(iVar8 + 0x34) < *(int *)(unaff_A5 + 0x4292) ||
                 (*(int *)(unaff_A5 + 0x429e) < *(int *)(iVar8 + 0x30))) ||
                (*(int *)(iVar8 + 0x38) < *(int *)(unaff_A5 + 0x4296)))))) {
          iVar8 = iVar8 + -0x40;
          sVar7 = sVar7 + -1;
          cVar6 = cVar6 * '\x02';
          if (cVar6 == '\0') {
            *(short *)(unaff_A5 + 0x42a4) = sVar7;
            *(undefined1 *)(unaff_A5 + 0x42a2) = 0;
            return CONCAT44(in_D0,in_D1);
          }
        }
        *(short *)(unaff_A5 + 0x42a4) = sVar7 + -1;
        *(char *)(unaff_A5 + 0x42a2) = cVar6 * '\x02';
        return CONCAT44(in_D0,in_D1);
      }
      piVar9 = piVar9 + 1;
      *(int **)(unaff_A5 + 0x406a) = piVar9;
      iVar1 = *(int *)(iVar8 + 0x2e);
    } while ((iVar1 == 0) || (*(int *)(iVar1 + 4) == 0));
    if (*(short *)(iVar1 + 0xc) == 0) break;
    iVar4 = unaff_D3 + in_D1;
    iVar2 = in_D1;
    if (in_A0 != 0) {
      iVar2 = *(int *)(in_A0 + 0x20) + in_D1;
      iVar4 = *(int *)(in_A0 + 0x18) + iVar4;
    }
    sVar7 = (short)((uint)iVar2 >> 0x10);
    sVar5 = (short)((uint)iVar4 >> 0x10);
    sVar3 = sVar7;
    if (sVar5 < sVar7) {
      sVar3 = sVar5;
      sVar5 = sVar7;
    }
    sVar7 = *(short *)(iVar8 + 0x18) +
            ((*(short *)(iVar8 + 0x34) - *(short *)(iVar1 + 0x10)) + 1) * 0x10;
  } while ((sVar7 < sVar3) || (sVar5 < (short)(sVar7 + *(short *)(iVar1 + 0x14) * -0x10)));
  return CONCAT44(in_D0,in_D1);
}
