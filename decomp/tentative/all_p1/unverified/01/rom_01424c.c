/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01424c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_1424c(void)

{
  int iVar1;
  int iVar2;
  undefined4 in_D0;
  undefined4 in_D1;
  char cVar3;
  short sVar4;
  int iVar5;
  int unaff_A5;
  int unaff_A6;

  cVar3 = *(char *)(unaff_A5 + 0x42a2);
  if (cVar3 == '\0') {
    return CONCAT44(in_D0,in_D1);
  }
  sVar4 = *(short *)(unaff_A5 + 0x42a4);
  iVar5 = unaff_A5 + 0x406e + (int)(short)(sVar4 << 6);
  iVar2 = unaff_A6 + 0x28;
  while ((((((-1 < cVar3 || (*(char *)(unaff_A6 + 0x6a) == (char)sVar4)) ||
            (*(short *)(iVar5 + 0x26) == 0)) ||
           ((iVar1 = *(int *)(iVar5 + 0x10), iVar1 != 0 &&
            ((iVar2 == iVar1 ||
             ((iVar1 = *(int *)(iVar1 + 0x10), iVar1 != 0 &&
              ((iVar2 == iVar1 ||
               ((*(int *)(iVar1 + 0x10) != 0 && (iVar2 == *(int *)(iVar1 + 0x10))))))))))))) ||
          (*(int *)(unaff_A5 + 0x429a) < *(int *)(iVar5 + 0x2c))) ||
         (((*(int *)(iVar5 + 0x34) < *(int *)(unaff_A5 + 0x4292) ||
           (*(int *)(unaff_A5 + 0x429e) < *(int *)(iVar5 + 0x30))) ||
          (*(int *)(iVar5 + 0x38) < *(int *)(unaff_A5 + 0x4296)))))) {
    iVar5 = iVar5 + -0x40;
    sVar4 = sVar4 + -1;
    cVar3 = cVar3 * '\x02';
    if (cVar3 == '\0') {
      *(short *)(unaff_A5 + 0x42a4) = sVar4;
      *(undefined1 *)(unaff_A5 + 0x42a2) = 0;
      return CONCAT44(in_D0,in_D1);
    }
  }
  *(short *)(unaff_A5 + 0x42a4) = sVar4 + -1;
  *(char *)(unaff_A5 + 0x42a2) = cVar3 * '\x02';
  return CONCAT44(in_D0,in_D1);
}
