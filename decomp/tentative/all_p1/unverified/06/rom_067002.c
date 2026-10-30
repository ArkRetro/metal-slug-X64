/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x067002.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_67002(void)

{
  undefined4 uVar1;
  undefined1 *puVar2;
  int *piVar3;
  undefined1 in_D0b;
  ushort uVar4;
  char cVar5;
  byte bVar6;
  undefined1 in_D1b;
  int *in_A0;
  int iVar7;
  int *piVar8;
  int unaff_A5;
  ushort uStack_6;

  _DAT_0010c586 = _DAT_0010c582;
  uVar1 = *(undefined4 *)(unaff_A5 + 0x4586);
  if ((int *)((int)_DAT_0010c582 + -0x10) < (int *)0x10c482) {
    return;
  }
  puVar2 = (undefined1 *)((int)_DAT_0010c582 + -7);
  _DAT_0010c582 = (int *)((int)_DAT_0010c582 + -0x10);
  *puVar2 = 0xff;
  *(undefined1 *)((int)_DAT_0010c586 + -6) = 0xff;
  *(undefined1 *)((int)_DAT_0010c586 + -5) = 0;
  *(undefined1 *)((int)_DAT_0010c586 + -4) = 0;
  if (_DAT_0010c586 < &DAT_0010c582) {
    candidate_66ac2();
    candidate_66d84();
  }
  piVar3 = _DAT_0010c582;
  _DAT_0010c586 = _DAT_0010c582;
  *_DAT_0010c582 = (int)in_A0;
  *(undefined1 *)((int)piVar3 + 9) = in_D0b;
  *(undefined1 *)((int)piVar3 + 10) = in_D1b;
  *(undefined1 *)((int)piVar3 + 7) = 0;
  *(undefined1 *)(piVar3 + 2) = 0;
  *(undefined1 *)((int)piVar3 + 0xf) = 0;
  *(undefined1 *)((int)piVar3 + 0xd) = 3;
  *(undefined1 *)((int)piVar3 + 0xe) = 0xff;
  *(undefined1 *)((int)piVar3 + 0xb) = *(undefined1 *)(in_A0 + 1);
  *(undefined1 *)(piVar3 + 3) = *(undefined1 *)((int)in_A0 + 5);
  *(undefined1 *)((int)piVar3 + 6) = 0;
  uStack_6 = 0;
  if (*in_A0 != 0) {
    uStack_6 = candidate_3d22a();
  }
  piVar8 = in_A0 + 4;
  iVar7 = *piVar8;
  while (iVar7 != -1) {
    if ((iVar7 != 0) && (iVar7 != -2)) {
      uVar4 = candidate_3d22a();
      if (uStack_6 < uVar4) {
        uStack_6 = uVar4;
      }
    }
    *(char *)((int)piVar3 + 6) = *(char *)((int)piVar3 + 6) + '\x01';
    piVar8 = piVar8 + 2;
    iVar7 = *piVar8;
  }
  if ((*(ushort *)(*piVar3 + 0xe) & 0xe) == 0) {
    cVar5 = '\x02';
  }
  else {
    cVar5 = '\0';
  }
  bVar6 = (char)uStack_6 + cVar5 + '\x02';
  if (*(byte *)((int)piVar3 + 0xb) < bVar6) {
    *(byte *)((int)piVar3 + 0xb) = bVar6;
  }
  bVar6 = *(char *)((int)piVar3 + 6) + 2;
  if (*(byte *)(piVar3 + 3) < bVar6) {
    *(byte *)(piVar3 + 3) = bVar6;
  }
  *(undefined1 *)(piVar3 + 1) = 0;
  uRam0010c58f = 1;
  _DAT_0010c586 = (int *)uVar1;
  return;
}
