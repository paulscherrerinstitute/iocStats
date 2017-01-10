#!/usr/bin/env python
import json
import lxml.etree as etree
import requests
import operator
import logging
import argparse
import os
import copy



def generate_panel(items,
                   tmplfile="IOC_OVERVIEW.ui.tmpl",
                   outfile="IOC_OVERVIEW.ui"):
    """
    Generate overview panel for a list of iocs. It takes tmplfile, searches for layout with name
    '[items.keys()_grid]' and inserts buttons and status indicators for each ioc.

    Args:
        items: dict of IOCs grouped by tabs (keys)
        tmplfile: template file
        outfile: outputfile
    """
    ioc_ui_str = """
                <item row="{0}" column="{1}">
                    <widget class="QWidget" name="widget" native="true">
                        <property name="geometry">
                            <rect>
                                <x>0</x>
                                <y>0</y>
                                <width>217</width>
                                <height>32</height>
                            </rect>
                        </property>
                        <property name="minimumSize">
                            <size>
                                <width>217</width>
                                <height>32</height>
                            </size>
                        </property>
                        <widget class="caRelatedDisplay" name="carelateddisplay">
                            <property name="geometry">
                                <rect>
                                    <x>6</x>
                                    <y>6</y>
                                    <width>142</width>
                                    <height>20</height>
                                </rect>
                            </property>
                            <property name="label">
                                <string notr="true" />
                            </property>
                            <property name="labels">
                                <string>{2}</string>
                            </property>
                            <property name="files">
                                <string>iocStats.ui</string>
                            </property>
                            <property name="args">
                                <string>IOC={2}</string>
                            </property>
                        </widget>
                        <widget class="caLineEdit" name="calineedit">
                            <property name="enabled">
                                <bool>true</bool>
                            </property>
                            <property name="geometry">
                                <rect>
                                    <x>0</x>
                                    <y>0</y>
                                    <width>217</width>
                                    <height>32</height>
                                </rect>
                            </property>
                            <property name="echoMode">
                                <enum>QLineEdit::NoEcho</enum>
                            </property>
                            <property name="channel" stdset="0">
                                <string notr="true">{2}:ALARM</string>
                            </property>
                            <property name="colorMode">
                                <enum>caLineEdit::Alarm_Static</enum>
                            </property>
                            <property name="alarmHandling">
                                <enum>caLineEdit::onBackground</enum>
                            </property>
                        </widget>
                        <widget class="caLineEdit" name="calineedit_2">
                            <property name="geometry">
                                <rect>
                                    <x>150</x>
                                    <y>6</y>
                                    <width>61</width>
                                    <height>20</height>
                                </rect>
                            </property>
                        </widget>
                        <zorder>calineedit</zorder>
                        <zorder>carelateddisplay</zorder>
                        <zorder>calineedit_2</zorder>
                    </widget>
                </item>"""

    # Empty Tab with grid layout.
    tab_str = """
              <widget class="QWidget" name="tab">
                  <attribute name="title">
                      <string>{}</string>
                  </attribute>
                  <widget class="QWidget" name="gridLayoutWidget">
                      <property name="geometry">
                          <rect>
                              <x>0</x>
                              <y>0</y>
                              <width>1100</width>
                              <height>591</height>
                          </rect>
                      </property>
                      <layout class="QGridLayout" name="grid" />
                  </widget>
              </widget>"""

    vertical_spacer_str = """
                          <item row="{}" column="{}">
                              <spacer name="verticalSpacer">
                                  <property name="orientation">
                                      <enum>Qt::Vertical</enum>
                                  </property>
                                  <property name="sizeHint" stdset="0">
                                      <size>
                                          <width>20</width>
                                          <height>40</height>
                                      </size>
                                  </property>
                              </spacer>
                          </item>"""

    tree = etree.parse(tmplfile)

    tab_order = items.pop('_tab_order_', None)
    if not tab_order:
      tab_order = list(items.keys()).sort() ## Default alphabetical

    for tab_name in tab_order:
      iocs = items.get(tab_name, None)
      if iocs:
        tabs_xml = tree.find("//widget[@name='tabWidget']")
        tab_xml = etree.fromstring(tab_str.format(tab_name))
        layout = tab_xml.find(".//layout[@class='QGridLayout']")
        tabs_xml.append(tab_xml)

        if layout is not None:
          i = 0
          for ioc in iocs:
            layout.append(etree.fromstring(ioc_ui_str.format(i%16, int(i/16), ioc['name'])))
            i += 1

          layout.append(etree.fromstring(vertical_spacer_str.format(i, 0)))

    print("Writing to "+ outfile)
    open(outfile, 'wb')
    tree.write(outfile)

def parse_ioc_name(ioc_name):
  # Prefix of PV is IOC name. Prefixes are by naming convention in following format (for SwissFEL):
  #  A-B-C where:
  #    A -> Sxxaabb
  #         - S: kingdom (S for SwissFEL)
  #         - xx: domain
  #         - aa (optional): sub-domain
  #         - bb (optional): section
  #
  #    B -> Cyyyzzz
  #         - C: group (C for controls ... IOCs are running on controls iocs)
  #         - yyy: device type
  #         - zzz (optional): position number (does not apply on IOC names)
  # 
  #    C -> Suffix (optional)  ... in case of IOC names it can gives some additional
  #                                info like (MOT for motion systems)
  ioc = {'name': ioc_name}

  # Parse IOC name to attributes which are used to generate different views
  name_parts = ioc_name.split('-', 2)
  ioc['domain'] = name_parts[0][:3]

  sub_domain = name_parts[0][3:5]
  if len(sub_domain)==2: # Check if defined, since it is optional
      ioc['sub_domain'] = sub_domain

  section = name_parts[0][5:7]
  if len(section)==2: # Check if defined, since it is optional
      ioc['section'] = section

  if len(name_parts) >=2:
    ioc['type'] = name_parts[1]
  else:
    ioc['type'] = None

  if len(name_parts) >=3:
    ioc['suffix'] = name_parts[2]
  else:
    ioc['suffix'] = None

  return ioc

def sort_iocs(iocs, tabs, mode='domain'):
  # Different views:
  #       - domain/section view

  # Sorting by domain/section can be done directly using ioc_name
  # First sort by name
  sorted_iocs = sorted(iocs, key=operator.itemgetter('name'))

  sorted_iocs = {k: [] for k in tabs}

  for ioc in iocs:
      item = ioc[mode]
      if item in sorted_iocs:
        sorted_iocs[item].append(ioc)

  # remove empty entries
  sorted_iocs_c = copy.copy(sorted_iocs)
  for group, iocs in sorted_iocs_c.items():
    if not iocs:
      sorted_iocs.pop(group)

  sorted_iocs['_tab_order_'] = tabs
  return sorted_iocs

###########
def main():
    logging.basicConfig(level=logging.WARN, format='[%(levelname)s] %(message)s')
    
    # Command line argument parsing
    parser = argparse.ArgumentParser(description='Tool generating iocStats overview panel for SwissFEL.\n\n' + \
                                           'By default it finds all IOCs with iocStats module and ' + \
                                           'sorts them by accelerator sections (SIN, S10, ...).\n\n' + \
                                           'Using --sort flag, IOCs can also be sorted by the device type.\n\n' + \
                                           'Groups and corresponding IOCs can also be defined manually ' + \
                                           'using JSON file and option --cfgfile.\nJSON file format:\n' + \
                                           '{\n    "_tab_order_":["LLRF", "DI"],\n    "LLRF": ' + \
                                           '["SIN-CPPM-TST1", "S20-CPPM-TST2"],\n    "DI": ["S10-CPPM-TST1", ' + \
                                           '"S20-CPPM-TST2"]\n}',
                                     formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('-o', '--outfile', default='./IOC_OVERVIEW.ui', 
                        help="(default: IOC_OVERVIEW.ui) Output ui file.")

    parser.add_argument('--sort', type=str, default='domain', choices = ['domain', 'type'],
                        help='(default: domain) Sort IOCs by:\n    domain (SIN, S10, S20, ...),\n' +\
                             '    type (CVME, CPPM, CPCL, ...)')

    parser.add_argument('-c', '--cfgfile', type=str, help='JSON file defining groups and IOCs (--sort ignored)')

    args = parser.parse_args()

    if args.cfgfile is not None:
      f = open(args.cfgfile)
      sorted_iocs = json.load(f)
      f.close()
      
      for group, iocs in sorted_iocs.items():
        parsed = list()
        if group != '_tab_order_':
          for ioc in iocs:
            parsed.append(parse_ioc_name(ioc))
          sorted_iocs[group] = parsed
    
    else:
      # Following query returns all PV names ending with IOC_CPU_LOAD
      r = requests.get("http://epics-boot-info.psi.ch/find-channel.aspx/IOC_CPU_LOAD?source=active&limit=0&facility=SWISSFEL&format=json&header=no")

      pv_list = json.loads(r.content.decode())
      
      ####
      # Valid sections by naming convention document
      domains = ['STS', 'SIN', 'S10', 'S20', 'S30', 'SAR', 'SAT', 'SGE', 'SLA', 'STH', 'SUH', 'SDA']
      types = ['CPCL', 'CPCW', 'CPPM', 'CVME', 'CSSU', 'CPPS', 'CMON', 'CWAG', 'CGPB', 'CMOV']

      prod_iocs = list()  # avoid duplication of IOCs (however it should not be the case)

      for pv in pv_list: 
          # Get IOC name and filter out non-production IOCs
          ioc = parse_ioc_name(pv['Channel'].split(':')[0])

          if ioc['name'] in prod_iocs:
              # avoid duplication of IOCs (however it should not be the case)
              logging.warning('IOC {} already on the list.'.format(ioc['name']))

          elif not ioc['domain'] in domains or ioc['type'] not in types:
              # Non-production IOC
              logging.warning('IOC skipped! IOC {} does not follow naming convention.'.format(ioc['name']))

          else:
              # valid ioc
              prod_iocs.append(ioc)

      # Different views:
      #       - domain/section view
      #       - IOC type view

      # Sorting by domain/section can be done directly using ioc_name
      if args.sort == 'domain':
        tabs = domains

      elif args.sort == 'type':
        tabs = types

      sorted_iocs = sort_iocs(prod_iocs, tabs, mode=args.sort)

    generate_panel(sorted_iocs, outfile=args.outfile)

if __name__ == "__main__":
    main()
