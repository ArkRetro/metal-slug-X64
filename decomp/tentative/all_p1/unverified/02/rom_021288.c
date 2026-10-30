/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021288.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21288(void)

{
  uint uVar1;
  ushort uVar2;
  ushort uVar3;
  byte *pbVar4;
  int unaff_A6;

  candidate_1abc8();
  uVar3 = *(ushort *)(unaff_A6 + 0xa8);
  pbVar4 = *(byte **)(unaff_A6 + 0xa0);
code_r0x00021296:
  if (10 < *pbVar4) {
    __m68k_trap(0xf);
  }
  switch(*pbVar4 & 0xf) {
  case 0:
    *(ushort *)(unaff_A6 + 0xaa) = (ushort)pbVar4[1] << 8;
    pbVar4 = pbVar4 + 2;
    *(byte **)(unaff_A6 + 0xa0) = pbVar4;
    goto code_r0x00021296;
  case 1:
    if (uVar3 == 0) {
      return;
    }
    uVar1 = (uint)*(ushort *)(pbVar4 + 2) * 0x10000 - (*(uint *)(unaff_A6 + 0xa4) & 0xffffff00);
    if ((uVar1 < 0x1000000) && (uVar2 = (ushort)(uVar1 >> 8), uVar2 <= uVar3)) {
      uVar3 = uVar3 - uVar2;
      *(undefined1 *)(unaff_A6 + 0xa6) = 0;
      *(undefined2 *)(unaff_A6 + 0xa4) = 0;
      func_0x000214b4();
      pbVar4 = pbVar4 + 4;
      *(byte **)(unaff_A6 + 0xa0) = pbVar4;
      goto code_r0x00021296;
    }
    break;
  case 2:
    if (uVar3 == 0) {
      return;
    }
    uVar1 = (((uint)*(ushort *)(pbVar4 + 4) << 0x10) >> 8) -
            (*(uint *)(unaff_A6 + 0xa4) & 0xffffff00);
    if ((uVar1 < 0x1000000) && (uVar2 = (ushort)(uVar1 >> 8), uVar2 <= uVar3)) {
      uVar3 = uVar3 - uVar2;
      *(undefined1 *)(unaff_A6 + 0xa6) = 0;
      *(undefined2 *)(unaff_A6 + 0xa4) = 0;
      func_0x000214b4();
      *(char *)(unaff_A6 + 0xaa) = pbVar4[1] + *(char *)(unaff_A6 + 0xaa);
      if (*(byte *)(unaff_A6 + 0xaa) == pbVar4[2]) {
        pbVar4 = pbVar4 + 6;
        *(byte **)(unaff_A6 + 0xa0) = pbVar4;
      }
      goto code_r0x00021296;
    }
    break;
  case 3:
    *(short *)(unaff_A6 + 0xa4) = *(short *)(unaff_A6 + 0xa4) + 1;
    if (*(ushort *)(pbVar4 + 2) <= *(ushort *)(unaff_A6 + 0xa4)) {
      *(undefined2 *)(unaff_A6 + 0xa4) = 0;
      *(byte **)(unaff_A6 + 0xa0) = pbVar4 + 4;
    }
    return;
  case 4:
    pbVar4 = *(byte **)(pbVar4 + 2);
    *(byte **)(unaff_A6 + 0xa0) = pbVar4;
    goto code_r0x00021296;
  case 5:
    if (0xaf < *(ushort *)(pbVar4 + 2)) {
      __m68k_trap(0xf);
    }
    *(undefined4 *)(unaff_A6 + (short)*(ushort *)(pbVar4 + 2)) = *(undefined4 *)(pbVar4 + 4);
    pbVar4 = pbVar4 + 8;
    *(byte **)(unaff_A6 + 0xa0) = pbVar4;
    goto code_r0x00021296;
  case 6:
    if (0xaf < *(ushort *)(pbVar4 + 4)) {
      __m68k_trap(0xf);
    }
    *(undefined2 *)(unaff_A6 + (short)*(ushort *)(pbVar4 + 4)) = *(undefined2 *)(pbVar4 + 2);
    pbVar4 = pbVar4 + 6;
    *(byte **)(unaff_A6 + 0xa0) = pbVar4;
    goto code_r0x00021296;
  case 7:
    if (0xaf < *(ushort *)(pbVar4 + 2)) {
      __m68k_trap(0xf);
    }
    *(byte *)(unaff_A6 + (short)*(ushort *)(pbVar4 + 2)) = pbVar4[1];
    pbVar4 = pbVar4 + 4;
    *(byte **)(unaff_A6 + 0xa0) = pbVar4;
    goto code_r0x00021296;
  case 8:
    (**(code **)(pbVar4 + 2))();
    pbVar4 = pbVar4 + 6;
    *(byte **)(unaff_A6 + 0xa0) = pbVar4;
    goto code_r0x00021296;
  case 9:
    goto code_r0x0002132a;
  case 10:
    func_0x000214b4();
    return;
  default:
    __m68k_trap(0xf);
    return;
  }
  *(int *)(unaff_A6 + 0xa4) = (uint)uVar3 * 0x100 + *(int *)(unaff_A6 + 0xa4);
  func_0x000214b4();
  return;
code_r0x0002132a:
  candidate_120f8();
  pbVar4 = pbVar4 + 4;
  *(byte **)(unaff_A6 + 0xa0) = pbVar4;
  goto code_r0x00021296;
}
