/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x005152.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5152(void)

{
  ushort uVar1;
  undefined2 *puVar2;
  ushort uVar3;
  short sVar4;
  short sVar5;
  undefined2 *puVar6;
  undefined2 *puVar7;

  puVar7 = (undefined2 *)0x1078fa;
  sVar5 = 0x100;
  sVar4 = 4;
  puVar2 = (undefined2 *)0x1068ea;
  do {
    puVar6 = puVar2;
    puVar6[2] = sVar4;
    puVar6[3] = sVar4;
    sVar4 = sVar4 + 0x10;
    puVar6[4] = 0;
    puVar6[5] = 0xffff;
    puVar6[6] = 0xffff;
    *(undefined1 *)(puVar6 + 7) = 0;
    *(undefined1 *)((int)puVar6 + 0xf) = 2;
    *puVar7 = 0;
    puVar7 = puVar7 + 0x10;
    sVar5 = sVar5 + -1;
    puVar2 = puVar6 + 8;
  } while (sVar5 != 0);
  puVar6[8] = 0x1000;
  puVar6[9] = 0x1000;
  uVar3 = 0x1000;
  uVar1 = 0xff0;
  do {
    uRam001078ec = uVar1;
    *(ushort *)((short)uVar3 + 0x1068ea) = uRam001078ec;
    *(ushort *)((short)uRam001078ec + 0x1068ec) = uVar3;
    uVar3 = uRam001078ec;
    uVar1 = uRam001078ec - 0x10;
  } while (0xf < uRam001078ec);
  uRam001068ea = 0x1000;
  uRam0010991a = 0;
  uRam0010991c = 0;
  uRam0010991e = 0;
  uRam00109920 = 0;
  uRam00109922 = 0;
  uRam00109924 = 0;
  uRam0010992c = 0;
  uRam00109942 = 0;
  uRam00109944 = 0;
  uRam00109945 = 0;
  return;
}
