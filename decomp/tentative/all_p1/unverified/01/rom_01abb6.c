/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01abb6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1abb6(void)

{
  int iVar1;
  int in_D0;
  int iVar2;
  ushort uVar3;
  int in_D1;
  int iVar4;
  ushort uVar5;
  int iVar6;
  int iVar7;
  byte bVar8;
  int *in_A0;
  int in_A1;

  in_A0[4] = in_A1;
  *in_A0 = in_D0;
  in_A0[1] = in_D1;
  *(undefined2 *)(in_A0 + 9) = 0;
  iVar6 = in_A0[5];
  iVar7 = in_A0[6];
  iVar1 = in_A0[4];
  iVar2 = *in_A0;
  iVar4 = in_A0[1];
  bVar8 = 0;
  if (iVar1 != 0) {
    uVar5 = (ushort)iVar4;
    uVar3 = (ushort)iVar2;
    iVar2 = *(int *)(iVar1 + 0x14) + iVar2;
    iVar4 = *(int *)(iVar1 + 0x18) + iVar4;
    bVar8 = *(byte *)(iVar1 + 0x25) |
            CARRY2((ushort)*(int *)(iVar1 + 0x18),uVar5) * '\x02' +
            CARRY2((ushort)*(int *)(iVar1 + 0x14),uVar3);
  }
  *(byte *)((int)in_A0 + 0x25) = bVar8;
  in_A0[5] = iVar2;
  in_A0[6] = iVar4;
  if (*(char *)(in_A0 + 9) == '\0') {
    *(char *)(in_A0 + 9) = cRam0010c33a;
    in_A0[7] = iVar2;
    in_A0[8] = iVar4;
    return;
  }
  if (cRam0010c33a != *(char *)(in_A0 + 9)) {
    *(char *)(in_A0 + 9) = cRam0010c33a;
    in_A0[7] = iVar6;
    in_A0[8] = iVar7;
  }
  return;
}
