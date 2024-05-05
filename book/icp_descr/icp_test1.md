\page test1_icp Test1 ICP
\par Usage
You can construct a new instance of Test1 ICP with `icp::ICP::from_method("test1", config)`. Supply the following parameters to `config` (via icp::ICP::Config::set):

Key | Description
--- | ---
`"overlap_rate"` | A `double` between 0 and 1 for the                  * overlap rate. The default is 1. 

\par Description
This is a WIP.

1.     **Matching Step**: see \ref vanilla_icp
    for details.

2.     **Trimming Step**: see \ref trimmed_icp for details.

3.     **Transformation Step**


Read \ref icp_sources for a list of all resources used in this project.
This page was automatically generated from test1.cpp with icp_doc_builder.py.