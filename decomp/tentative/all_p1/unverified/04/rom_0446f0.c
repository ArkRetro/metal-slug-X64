/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0446f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_446f0(void)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  ushort uVar5;
  undefined4 *puVar6;
  int unaff_A6;
  undefined8 uVar7;
  ushort in_stack_fffffff6;
  undefined2 in_stack_fffffffa;
  short *psVar8;

  psVar8 = (short *)(unaff_A6 + 0x28);
  bVar1 = *(byte *)(unaff_A6 + 0x81);
  if ((char)bVar1 < '\0') {
    if (*(char *)(unaff_A6 + 0x82) == '\0') {
      *(byte *)(unaff_A6 + 0x81) = *(byte *)(unaff_A6 + 0x81) & 0x7f;
      *(undefined1 *)(unaff_A6 + 0x82) = 0;
    }
    else {
      *(char *)(unaff_A6 + 0x82) = *(char *)(unaff_A6 + 0x82) + -1;
    }
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    *(undefined4 *)(unaff_A6 + 0x34) = 0;
    return (uint)bVar1;
  }
                    /* WARNING: Switch is manually overridden */
  switch(bVar1) {
  case 0:
    return (uint)bVar1;
  case 4:
    uVar5 = *(short *)(unaff_A6 + 0x7a) - *psVar8;
    puVar6 = (undefined4 *)
             CONCAT22(*(short *)(unaff_A6 + 0x7c) - *(short *)(unaff_A6 + 0x2c),in_stack_fffffffa);
    candidate_2eb4();
    uVar7 = candidate_2e7c();
    goto LAB_00044772;
  case 8:
    break;
  case 0xc:
  }
  uVar5 = *(short *)(unaff_A6 + 0x7a) - *psVar8;
  puVar6 = (undefined4 *)
           CONCAT22(*(short *)(unaff_A6 + 0x7c) - *(short *)(unaff_A6 + 0x2c),in_stack_fffffffa);
  candidate_2eb4();
  uVar7 = candidate_2e7c(uVar5,puVar6,psVar8);
LAB_00044772:
  uVar2 = (uint)((ulonglong)uVar7 >> 0x20);
  uVar4 = (uint)uVar7;
  if (-1 < (int)(uVar2 ^ uVar2 + (uint)uVar5 * -0x10000)) {
    uVar2 = 0;
    *puVar6 = *(undefined4 *)(unaff_A6 + 0x7a);
  }
  if (-1 < (int)(uVar4 ^ uVar4 + (uint)in_stack_fffffff6 * -0x10000)) {
    uVar4 = 0;
    puVar6[1] = *(undefined4 *)(unaff_A6 + 0x7c);
  }
  puVar6[2] = uVar2;
  puVar6[3] = uVar4;
  uVar3 = uVar4 | uVar2;
  if ((uVar4 | uVar2) == 0) {
    *(undefined1 *)(unaff_A6 + 0x81) = 0;
    return uVar3;
  }
  return uVar3;
}
